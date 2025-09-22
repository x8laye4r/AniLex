import json
import mimetypes
import os
from typing import Dict, List, Optional

import requests


class SauceNaoAPI:
    """
    A client for the SauceNAO API that provides image source searching capabilities.

    SauceNAO is a reverse image search engine that specializes in finding the source
    of anime, manga, and other illustrations.
    """

    BASE_URL = "https://saucenao.com/search.php"

    def __init__(self, api_key: str, output_type: str = "2", numres: int = 5):
        """
        Initialize the SauceNAO API client.

        Args:
            api_key: Your SauceNAO API key
            output_type: Response output type (2 = JSON)
            numres: Number of results to return (max 32)
        """
        self.api_key = api_key
        self.output_type = output_type
        self.numres = numres
        self.last_response = None
        self.results = []

    def search_by_url(self, image_url: str, db_mask: Optional[int] = None) -> Dict:
        """
        Search for an image using its URL.

        Args:
            image_url: URL of the image to search
            db_mask: Optional database mask to filter results

        Returns:
            Dict containing the parsed response
        """
        params = {
            'api_key': self.api_key,
            'output_type': self.output_type,
            'numres': self.numres,
            'url': image_url
        }

        if db_mask:
            params['dbmask'] = db_mask

        response = requests.get(self.BASE_URL, params=params)
        return self._process_response(response)

    def search_by_file(self, file_path: str, db_mask: Optional[int] = None) -> Dict:
        """
        Search for an image by uploading a local file.

        Args:
            file_path: Path to the local image file
            db_mask: Optional database mask to filter results

        Returns:
            Dict containing the parsed response
        """
        if not os.path.isfile(file_path):
            raise FileNotFoundError(f"File not found: {file_path}")

        mime_type = mimetypes.guess_type(file_path)[0]
        if not mime_type or not mime_type.startswith('image/'):
            raise ValueError(f"File does not appear to be an image: {file_path}")

        files = {'file': (os.path.basename(file_path), open(file_path, 'rb'), mime_type)}
        data = {
            'api_key': self.api_key,
            'output_type': self.output_type,
            'numres': self.numres
        }

        if db_mask:
            data['dbmask'] = db_mask

        response = requests.post(self.BASE_URL, files=files, data=data)
        return self._process_response(response)

    def _process_response(self, response: requests.Response) -> Dict:
        """
        Process the API response and store results.

        Args:
            response: Response object from the requests library

        Returns:
            Dict containing the parsed response
        """
        try:
            response.raise_for_status()
            data = response.json()
            self.last_response = data

            if data.get('header', {}).get('status') != 0:
                error_msg = data.get('header', {}).get('message', 'Unknown error')
                raise Exception(f"API Error: {error_msg}")

            self.results = self._parse_results(data.get('results', []))
            return data

        except requests.exceptions.RequestException as e:
            raise Exception(f"Request Error: {str(e)}")
        except json.JSONDecodeError:
            raise Exception("Failed to parse API response as JSON")

    def _parse_results(self, results: List[Dict]) -> List[Dict]:
        """
        Parse and normalize the results from the API.

        Args:
            results: Raw results from the API

        Returns:
            List of parsed and normalized result dictionaries
        """
        parsed_results = []

        for result in results:
            header = result.get('header', {})
            data = result.get('data', {})

            parsed_result = {
                'similarity': float(header.get('similarity', 0)),
                'thumbnail': header.get('thumbnail', ''),
                'index_id': header.get('index_id', 0),
                'index_name': header.get('index_name', ''),
                'source': data.get('source', ''),
                'title': data.get('title', ''),
                'ext_urls': data.get('ext_urls', []),
                'raw_data': data  # Include all raw data for advanced usage
            }

            # Add specific fields based on the index type
            if 'anidb_aid' in data:
                parsed_result['anidb_id'] = data.get('anidb_aid')
                parsed_result['type'] = 'anime'
                parsed_result['year'] = data.get('year', '')
                parsed_result['part'] = data.get('part', '')
                parsed_result['est_time'] = data.get('est_time', '')

            elif 'danbooru_id' in data or 'gelbooru_id' in data:
                parsed_result['type'] = 'booru'
                parsed_result['danbooru_id'] = data.get('danbooru_id', '')
                parsed_result['gelbooru_id'] = data.get('gelbooru_id', '')
                parsed_result['creator'] = data.get('creator', '')
                parsed_result['material'] = data.get('material', '')
                parsed_result['characters'] = data.get('characters', '')

            elif 'pixiv_id' in data:
                parsed_result['type'] = 'pixiv'
                parsed_result['pixiv_id'] = data.get('pixiv_id')
                parsed_result['member_name'] = data.get('member_name', '')
                parsed_result['member_id'] = data.get('member_id', '')

            parsed_results.append(parsed_result)

        return parsed_results

    def get_best_match(self) -> Optional[Dict]:
        """
        Get the best match from the search results.

        Returns:
            The result with the highest similarity or None if no results
        """
        if not self.results:
            return None

        return self.results[0]

    def get_all_matches(self) -> List[Dict]:
        """
        Get all matches from the search results.

        Returns:
            List of all result dictionaries
        """
        return self.results

    def get_matches_above_similarity(self, threshold: float) -> List[Dict]:
        """
        Get all matches above a certain similarity threshold.

        Args:
            threshold: Similarity threshold (0-100)

        Returns:
            List of results with similarity above the threshold
        """
        return [r for r in self.results if r['similarity'] >= threshold]

    def get_rate_limits(self) -> Dict:
        """
        Get information about API rate limits from the last response.

        Returns:
            Dictionary containing rate limit information
        """
        if not self.last_response:
            return {}

        header = self.last_response.get('header', {})
        return {
            'short_limit': header.get('short_remaining', 0),
            'long_limit': header.get('long_remaining', 0),
            'short_limit_ttl': header.get('short_limit_ttl', 0),
            'long_limit_ttl': header.get('long_limit_ttl', 0),
        }
